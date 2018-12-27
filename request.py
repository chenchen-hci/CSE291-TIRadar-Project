from urllib import request, parse
import json
import time as t
import requests

url = "http://127.0.0.1:5000"

def post(n1_cluster, n1_noise, n1_ratio, n2_cluster, n2_noise, n2_ratio, n3_cluster, n3_noise, n3_ratio, timestamp):
    print("post.......")
    # Dict to Json
    # Difference is { "test":10, "test2":20 }
    data_json = {
        "n1_cluster": n1_cluster,
        "n1_noise": n1_noise,
        "n1_ratio": n1_ratio,
        "n2_cluster": n2_cluster,
        "n2_noise": n2_noise,
        "n2_ratio": n2_ratio,
        "n3_cluster": n3_cluster,
        "n3_noise": n3_noise,
        "n3_ratio": n3_ratio,
        "timestamp": timestamp
    }

    data = json.dumps(data_json)

    # # Convert to String
    # data = str(data)
    #
    # # Convert string to byte
    # data = data.encode('utf-8')

    # Post Method is invoked if data != None
    print(data)

    try:
        requests.post(url, json=data)
    except Exception as ex:
        print(ex)
    # Response

def test ():
    print("post.......")
    # Dict to Json
    # Difference is { "test":10, "test2":20 }
    data_json = {
        "timestamp": t.time()
    }

    data = json.dumps(data_json)

    # # Convert to String
    # data = str(data)
    #
    # # Convert string to byte
    # data = data.encode('utf-8')

    # Post Method is invoked if data != None
    print(data)

    try:
        requests.post(url, json=data)
    except Exception as ex:
        print(ex)
    # Response

test()