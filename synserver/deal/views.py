#coding=utf-8
from django.shortcuts import render
from django.http import HttpResponse
from django.views.decorators.csrf import csrf_exempt
from deal.models import StartFlag

@csrf_exempt
def start(request):
	startFlag = StartFlag.objects.filter(id = 1);
	if startFlag:
		startFlag = startFlag[0]
		return HttpResponse(str(startFlag.flag));
	sf = StartFlag(id = 1, flag = 0);
	sf.save();
	return HttpResponse("0")

@csrf_exempt
def uploadinfor(request):
	return HttpResponse(u"14")
	
@csrf_exempt
def end(request):
	return HttpResponse(u"end")

@csrf_exempt
def controlstart(request):
	return HttpResponse(u"end")
	
@csrf_exempt
def getstatus(request):
	return HttpResponse(u"end")
