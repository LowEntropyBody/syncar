#coding=utf-8
from django.shortcuts import render
from django.http import HttpResponse
from django.views.decorators.csrf import csrf_exempt
from deal.models import StartFlag
from deal.models import DevAim

@csrf_exempt
def home(request):
	return render(request,'home.html');

@csrf_exempt
def start(request):
	devId = str(request.POST['devId']);
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
def getaimid(request):
	devId = str(request.POST['devId']);
	return HttpResponse(u"1")
	
@csrf_exempt
def end(request):
	return HttpResponse(u"end")

#启动系统
@csrf_exempt
def controlstart(request):
	startFlag = StartFlag.objects.filter(id = 1);
	if startFlag:
		startFlag = startFlag[0]
		startFlag.flag = 1;
		startFlag.save();
		return HttpResponse("System start");
	sf = StartFlag(id = 1, flag = 1);
	sf.save();
	return HttpResponse("System start")
	
@csrf_exempt
def getstatus(request):
	return HttpResponse(u"end")
