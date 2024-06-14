#!/bin/env python3
'''
获取随机壁纸，并播放在设备上。
'''
import requests
import tempfile
import os
import time
import subprocess
url = 'https://api.suyanw.cn/api/360bizhi.php?n=4'
response = requests.get(url)

if response.status_code == 200:
    data = response.json()
    pizhi_url = data['text']
    img_response = requests.get(pizhi_url)
    if img_response.status_code == 200:
        temp_file = tempfile.NamedTemporaryFile(delete=False)
        temp_file_name = temp_file.name
        temp_file.write(img_response.content)
        temp_file.close()
        newfile = temp_file_name+'.'+pizhi_url.split('.')[-1]
        os.rename(temp_file_name, newfile)
        print(f'display new image {newfile}')

        env = os.environ.copy()
        env['FRAMEBUFFER'] = '/dev/fb0'
        proc = subprocess.Popen(
            ['fbv', '-k', newfile],
            stdin=subprocess.PIPE,
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True,
            env=env
        )
        message = "\r\n"
        proc.stdin.write(message)
        proc.stdin.flush()
        output, errors = proc.communicate()
        # print("Output from subprocess:", output)
        # print("Errors from subprocess:", errors if errors else "No errors")
        # process = subprocess.Popen(['fbv', '-k', newfile], env=os.environ)
        # # subprocess.run(['fbv', '-k', newfile], env=os.environ)
        # time.sleep(2)
        # process.stdin.write(b'\r\n')
        # process.stdin.flush()
        # process.wait()
        # os.system(f'timeout 5s fbv -k {newfile}')
        os.remove(newfile)
else:
    print(f"requests faile,status:{response.status_code}")