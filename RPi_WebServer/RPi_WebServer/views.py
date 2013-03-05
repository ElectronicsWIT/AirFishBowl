#!/usr/bin/python
from django.http import HttpResponse
from django.template import RequestContext, loader

def home(request):
	t = loader.get_template("RPi_WebServer/index.html")
	c = RequestContext(request)
	return HttpResponse(t.render(c)) 
