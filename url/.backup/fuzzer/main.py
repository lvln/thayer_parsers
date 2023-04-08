import random
import string
import urllib.parse
import requests

# Simple base URL for fuzzing
base_url = "https://home.dartmouth.edu/"

def random_string(length):
    return ''.join(random.choice(string.ascii_letters + string.digits) for _ in range(length))

def random_parameter_value():
    return urllib.parse.quote(random_string(random.randint(1, 50)))

def random_path_component():
    return random_string(random.randint(1, 50))

# Generate the URI
def random_uri():
    # Include query string randomly
    if random.randint(0, 1):
        query = "?param1={}&param2={}".format(random_parameter_value(), random_parameter_value())
    else:
        query = ""
    # Include fragment identifier randomly
    if random.randint(0, 1):
        fragment = "#" + random_string(random.randint(1, 50))
    else:
        fragment = ""
    # Build URI
    uri = base_url + random_path_component() + query + fragment
    return uri

# Send a request
def send_request(uri):
    try:
        response = requests.get(uri)
        print("{} - {}".format(response.status_code, uri))
    except requests.exceptions.RequestException as e:
        print("Error: {}".format(e))

# Fuzz URI
while True:
    uri = random_uri()
    send_request(uri)
