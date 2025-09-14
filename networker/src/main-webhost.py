"""A simple web server"""

from micropyserver import MicroPyServer
import utils

TOP = """
<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8" />
<meta name="viewport" content="width=device-width, initial-scale=1" />
<style>
html, body {
  height: 100%;
}
body {
  display: flex;
  flex-direction: column;
  align-items: center;
  justify-content: center;
}
</style>
</head>
<body>
"""
BOTTOM = """
</body>
</html>
"""

MAIN_PAGE = (
    TOP
    + """
<form method="post" action="/submit">
    <input type="text" name="name" id="name">
    <button type="submit">SUBMIT</button>
</form>
"""
    + BOTTOM
)

SUBMIT_PAGE = (
    TOP
    + """
    thanks :]
    <a href="/">back</a>
    """
    + BOTTOM
)


def hello_world(request):
    """request handler"""
    print("new request!")
    print(request)
    query_params = utils.get_request_query_params(request)
    print(query_params)
    utils.send_response(server, MAIN_PAGE)


def submit(request):
    """submit handler"""
    print(request)
    query_params = utils.get_request_query_params(request)
    print(query_params)
    utils.send_response(server, SUBMIT_PAGE)


server = MicroPyServer()
server.add_route("/", hello_world)
server.add_route("/submit", submit, method="POST")
server.start()
