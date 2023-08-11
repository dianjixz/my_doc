#!/usr/bin/env python
# Usage: python3 SimpleHTTPSServer.py port

from OpenSSL import crypto, SSL
from socket import gethostname
from pprint import pprint
from time import gmtime, mktime
import os
import http.server, ssl
import argparse

parser = argparse.ArgumentParser(description="SimpleHTTPSServer \n Usage: python3 SimpleHTTPSServer.py 443")
parser.add_argument("port", type=int, default=443, nargs="?", help="Port Number, default is 443")
args = parser.parse_args()
CERT_PATH = "server.crt"
KEY_PATH = "server.key"
PEM_PATH = "server.pem"

def create_self_signed_cert(cert_path,key_path,pem_path):
    # create a key pair
    k = crypto.PKey()
    k.generate_key(crypto.TYPE_RSA, 1024)

    # create a self-signed cert
    cert = crypto.X509()
    cert.get_subject().C = "UK"
    cert.get_subject().ST = "London"
    cert.get_subject().L = "London"
    cert.get_subject().O = "Dummy Company Ltd"
    cert.get_subject().OU = "Dummy Company Ltd"
    cert.get_subject().CN = os.gethostname()
    cert.set_serial_number(1000)
    cert.gmtime_adj_notBefore(0)
    cert.gmtime_adj_notAfter(10*365*24*60*60)
    cert.set_issuer(cert.get_subject())
    cert.set_pubkey(k)
    cert.sign(k, 'sha1')
    if (not os.path.exists(cert_path)):
        open(cert_path, "wb").write(crypto.dump_certificate(crypto.FILETYPE_PEM, cert))
    if (not os.path.exists(key_path)):
        open(key_path, "wb").write(crypto.dump_privatekey(crypto.FILETYPE_PEM, k))
    if (not os.path.exists(pem_path)):
        open(pem_path, "wb").write(crypto.dump_privatekey(crypto.FILETYPE_PEM, k))
        open(pem_path, "ab").write(crypto.dump_certificate(crypto.FILETYPE_PEM, cert))

def create_https_server(cert_path,key_path,port):
    server_address = ('0.0.0.0', port)
    try:
        httpd = http.server.HTTPServer(server_address, http.server.SimpleHTTPRequestHandler)
        httpd.socket = ssl.wrap_socket(httpd.socket, server_side=True, certfile='server.pem', ssl_version=ssl.PROTOCOL_TLS)
    except:
        exit("[ERR] Port %d has been taken."%(port))
    print("Serving HTTPS on 0.0.0.0 port %d..."%(port))
    httpd.serve_forever()

if __name__ == "__main__":
    create_self_signed_cert(CERT_PATH,KEY_PATH,PEM_PATH)
    create_https_server(CERT_PATH,KEY_PATH,args.port)
