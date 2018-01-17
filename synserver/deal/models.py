from __future__ import unicode_literals
from django.db import models
#ecoding:utf-8

class StartFlag(models.Model):
	flag = models.IntegerField(default = 0);
	
class DevAim(models.Model):
	devId = models.CharField(max_length=100,null=False,default="0");
	aimId = models.CharField(max_length=100,null=False,default="0");
	distance = models.IntegerField(default = 0);
	aimSocre = models.IntegerField(default = 0);
