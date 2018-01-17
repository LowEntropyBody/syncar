from __future__ import unicode_literals
from django.db import models
#ecoding:utf-8

class StartFlag(models.Model):
	flag = models.IntegerField(default = 0);
	
class DevAim(models.Model):
	devId = models.IntegerField(default = 0);
	aimId = models.IntegerField(default = 0);
	distance = models.models.FloatField(default = 0.0);
	aimSocre = models.models.FloatField(default = 0.0);
