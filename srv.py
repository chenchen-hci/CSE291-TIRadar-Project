from flask import Flask, request
from flask_restful import Resource, Api
import json
import csv
import sys
import time as t

app = Flask(__name__)
api = Api(app)

filename = "timedata.log"
log = []

def file_check(filename):
    try:
      open(filename, "r")
      return True
    except IOError:
      return False

def write_data(data, filename):
    '''
    data is a N * 10 matrix
    :param data:
    :param filename:
    :return:
    '''
    if len(data) == 0:
        return False

    with open(filename + ".log", mode = 'a') as file:
        data_writer = csv.writer(file, delimiter=',')

        data_writer.writerow(data)
    return True

class Apis(Resource):
    def post(self):

        global log
        data = json.loads(request.data)
        data = data.replace("'", "\"")
        data = json.loads(data)

        # assemble data:
        log = [ \
              data["timestamp"], \
              data["n1_cluster"], \
              data["n1_noise"], \
              data["n1_ratio"],  \
              data["n2_cluster"], \
              data["n2_noise"], \
              data["n2_ratio"], \
              data["n3_cluster"], \
              data["n3_noise"], \
              data["n3_ratio"],  \
        ]
        # log the data
        print(log)
        write_data(log, filename)
        return {"ok": True}

    def get(self):
        global log
        if len(log) == 0:
            return {
                "ok": False,
                "n1_cluster": 0,
                "n1_noise": 0,
                "n1_ratio": 0,
                "n2_cluster": 0,
                "n2_noise": 0,
                "n2_ratio": 0,
                "n3_cluster": 0,
                "n3_noise": 0,
                "n3_ratio": 0,
                "timestamp": t.time()
            }
        else:
            return {
                "ok": True,
                "n1_cluster": log[1],
                "n1_noise": log[2],
                "n1_ratio": log[3],
                "n2_cluster": log[4],
                "n2_noise": log[5],
                "n2_ratio": log[6],
                "n3_cluster": log[7],
                "n3_noise": log[8],
                "n3_ratio": log[9],
                "timestamp": log[0]
            }

api.add_resource(Apis, '/')

if __name__ == '__main__':
    if file_check(filename) == False:
        print("[error] ", t.time(), " file ", filename, "does not exit")
        sys.exit(-1)

    app.run(port = 5000, debug=True)
