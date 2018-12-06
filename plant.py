#!/usr/bin/env python3

from flask import Flask
app = Flask(__name__)

DEBUG = True
HOST = '0.0.0.0'
PORT = 5000

@app.route("/")
def hello():
    return "Hello World!"


if __name__=="__main__":
    app.run(HOST, PORT, debug=DEBUG)
