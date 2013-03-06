#!/usr/bin/python
from django.conf import settings
from django.conf.urls import *
from views import *

urlpatterns = patterns('',
	url(r'^$', home, name='home')
)
