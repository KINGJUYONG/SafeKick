import re
import json
import time
import numpy as np
from .models import Timer, Profile
from django.shortcuts import render
from django.http import HttpResponse
from rest_framework.response import Response
from rest_framework.views import APIView
import os
import subprocess
from collections import OrderedDict
import random

BASE_PATH = "/home/hadoop/model/"

def get_client_ip(request):
    x_forwarded_for = request.META.get('HTTP_X_FORWARDED_FOR')
    if x_forwarded_for:
        ip = x_forwarded_for.split(',')[0]
    else:
        ip = request.META.get('REMOTE_ADDR')
    return ip


class ReflectAPI(APIView):
    def post(self, request):
        if request.method == 'POST':
            print("IP : ",get_client_ip(request))

            initt = json.loads(request.body)
            with open('test.txt', 'w', encoding='utf-8') as txtfile:
                    data = str(initt.get("helmet"))
                    #print(data + "의 값 저장됨")
                    txtfile.write(data)
            txtfile.close()

            hun = int(data) // 100
            ten = (int(data) - hun  * 100) // 10
            one = (int(data) - (hun  * 100 + ten * 10)) // 1
            
            Result = {
                "helmet_state" : hun,
                "two_riding" : one, # 수정 필요
                "accident" : ten, # @@@@@@@@@이것도 알고리즘 좀 손보기
                "wholeflags" : int(data)
            }

            print("응답 : " + str(Result))

            return Response("")

class helmetAPI(APIView):
    def post(self, request):
        if request.method == 'POST':
            print("IP : ",get_client_ip(request))

            with open('test.txt', encoding='utf-8') as txtfile:
                initial = txtfile.read()
            txtfile.close()

            hun = int(initial) // 100
            ten = (int(initial) - hun  * 100) // 10
            one = (int(initial) - (hun  * 100 + ten * 10)) // 1

            Result = {
                "helmet_state" : hun,
                "two_riding" : one, # 수정 필요
                "accident" : ten, # @@@@@@@@@이것도 알고리즘 좀 손보기
                "wholeflags" : int(initial)
            }

            return Response(Result)

            