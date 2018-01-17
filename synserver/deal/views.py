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
	devId = str(request.POST['devId']);
	d1 = str(request.POST['d1']);
	d2 = str(request.POST['d2']);
	d3 = str(request.POST['d3']);
	d4 = str(request.POST['d4']);
	d5 = str(request.POST['d5']);
	d6 = str(request.POST['d6']);
	print d1
	print d2
	print d3
	print d4
	print d5
	print d6
	devAim = DevAim.objects.get(devId = int(devId), aimId = 1);
	devAim.distance = d1
	devAim.save()
	devAim = DevAim.objects.get(devId = int(devId), aimId = 2);
	devAim.distance = d2
	devAim.save()
	devAim = DevAim.objects.get(devId = int(devId), aimId = 3);
	devAim.distance = d3
	devAim.save()
	devAim = DevAim.objects.get(devId = int(devId), aimId = 4);
	devAim.distance = d4
	devAim.save()
	devAim = DevAim.objects.get(devId = int(devId), aimId = 5);
	devAim.distance = d5
	devAim.save()
	devAim = DevAim.objects.get(devId = int(devId), aimId = 6);
	devAim.distance = d6
	devAim.save()
	
	return HttpResponse(u" success upload")
	
@csrf_exempt
def getaimid(request):
	devId = str(request.POST['devId']);
	devAim1 = DevAim.objects.get(devId = 1, aimId = 1);
	devAim2 = DevAim.objects.get(devId = 2, aimId = 1);
	devAim3 = DevAim.objects.get(devId = 3, aimId = 1);
	if devAim1.distance >= 0 and devAim2.distance >= 0 and devAim3.distance >= 0:
		return HttpResponse(u"2")
	return HttpResponse(u"1")
	
@csrf_exempt
def end(request):
	return HttpResponse(u"end")


@csrf_exempt
def controlstart(request):
	flag_temp = str(request.POST['flag']);
	
	startFlag = StartFlag.objects.get(id = 1);
	startFlag.flag = int(flag_temp);
	startFlag.save();
	
	devAim = DevAim.objects.filter(devId = 1);
	for da in devAim:
		da.distance = -2;
		da.save();

	
	devAim = DevAim.objects.filter(devId = 2);
	for da in devAim:
		da.distance = -2;
		da.save();
	
	devAim = DevAim.objects.filter(devId = 3);
	for da in devAim:
		da.distance = -2;
		da.save();

	return HttpResponse("System Start")
	
@csrf_exempt
def getstatus(request):
	return HttpResponse(u"end")
