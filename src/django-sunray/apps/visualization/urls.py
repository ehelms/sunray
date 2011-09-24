from django.conf.urls.defaults import *
from django.views.generic import TemplateView


urlpatterns = patterns('',
    url(r'^visualization/$', TemplateView.as_view(template_name="visualization/visual.html")),
)
