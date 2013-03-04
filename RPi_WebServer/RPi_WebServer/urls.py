#!/usr/bin/python
from django.conf import settings
from django.conf.urls.defaults import *
from views import *

urlpatterns = patterns('',
	url(r'^$', home, name='home')
)
