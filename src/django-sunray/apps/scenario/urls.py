from django.conf.urls.defaults import *
from django.views.generic import TemplateView


urlpatterns = patterns('',
    url(r'^scenario/$', TemplateView.as_view(template_name="scenario/new.html")),
)
