#!/usr/bin/env python3

import json

from flask import (Flask,
                   render_template,
                   request)

app = Flask(__name__)


DEBUG = True
HOST = '0.0.0.0'
PORT = 5000


METRICS = []


@app.route("/")
def hello():
    context = {}
    return render_template("index.html", **context)


@app.route("/metrics", methods=["GET"])
def metrics():
    return json.dumps({"data":METRICS[0:10]}), 200


@app.route("/metrics", methods=["POST"])
def new_metric():
    moisture = request.args.get('m', None)
    temperature = request.args.get('t', None)
    light = request.args.get('l', None)
    METRICS.append(
        (moisture, temperature, light)
    )
    print(f"received metric: moisture {moisture}, temperature {temperature}, light {light}")
    return "OK", 200


if __name__=="__main__":
    app.run(HOST, PORT, debug=DEBUG)
