#!/usr/bin/env python3

import collections
import json

from flask import (Flask,
                   render_template,
                   request)

app = Flask(__name__)


DEBUG = True
HOST = '0.0.0.0'
PORT = 5000


# prevent this from eating all my memory:
METRICS = collections.deque(maxlen=10)


# TODO Add a decorator that 403s non-http (X-Forwarded-Proto != https)
# requests to tell the user to retry the request on HTTPS.


@app.route("/")
def hello():
    context = {}
    return render_template("index.html", **context)


@app.route("/metrics", methods=["GET"])
def metrics():
    data = {
        "moisture": [],
        "temperature": [],
        "light": [],
    }
    for m, t, l in METRICS:
        data['moisture'].append(m)
        data['temperature'].append(t)
        data['light'].append(l)
    return app.response_class(
        json.dumps(data), status=200,
        mimetype='application/json'
    )


@app.route("/metrics", methods=["POST"])
def new_metric():
    moisture = request.args.get('m', None)
    if moisture:
        moisture = int(moisture)
    temperature = request.args.get('t', None)
    if temperature:
        temperature = float(temperature)
    light = request.args.get('l', None)
    if light:
        light = int(light)
    METRICS.append(
        (moisture, temperature, light)
    )
    print(f"received metric: moisture {moisture}, temperature {temperature}, light {light}")
    return "OK", 200


if __name__=="__main__":
    app.run(HOST, PORT, debug=DEBUG)
