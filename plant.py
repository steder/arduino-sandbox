#!/usr/bin/env python3

import collections
import json
import os
from urllib import parse 

from flask import (Flask,
                   render_template,
                   request)
import redis

url = parse.urlparse(os.environ.get('REDISCLOUD_URL', "redis://localhost:6379/0"))
db = redis.Redis(host=url.hostname, port=url.port, password=url.password)


app = Flask(__name__)


DEBUG = True
HOST = '0.0.0.0'
PORT = 5000

# Keep no more than 1000 records per device, for now:
CAPPED_SIZE = 1000 
devices_key = "devices"
metrics_template = "metrics:%s"
# TODO Add a decorator that 403s non-http (X-Forwarded-Proto != https)
# requests to tell the user to retry the request on HTTPS.


@app.route("/")
def hello():
    context = {}
    return render_template("index.html", **context)


@app.route("/metrics", methods=["GET"])
def metrics(deviceid="device1"):
    data = {
        "moisture": [],
        "temperature": [],
        "light": [],
    }
    metrics_key = metrics_template % (deviceid,)
    metrics = db.lrange(metrics_key, 0, 20)
    for row in metrics:
        m, t, l = row.decode("utf-8").split(",")
        data['moisture'].append(int(m))
        data['temperature'].append(float(t))
        data['light'].append(int(l))
    return app.response_class(
        json.dumps(data), status=200,
        mimetype='application/json'
    )


@app.route("/metrics", methods=["POST"])
def new_metric(deviceid="device1"):
    moisture = request.args.get('m', -1)
    if moisture:
        moisture = int(moisture)
    temperature = request.args.get('t', -100.0)
    if temperature:
        temperature = float(temperature)
    light = request.args.get('l', -1)
    if light:
        light = int(light)
    metrics_key = metrics_template % (deviceid,)
    db.sadd(devices_key, deviceid)
    db.rpush(metrics_key, "%i,%f,%i"%(moisture, temperature, light))
    db.ltrim(metrics_key, 0, CAPPED_SIZE) 
    print(f"received metric: moisture {moisture}, temperature {temperature}, light {light}")
    return "OK", 200


if __name__=="__main__":
    app.run(HOST, PORT, debug=DEBUG)
