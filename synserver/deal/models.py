from __future__ import unicode_literals
from django.db import models
#ecoding:utf-8

class StartFlag(models.Model):
	flag = models.IntegerField(default = 0);
