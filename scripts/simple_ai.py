#!/bin/env python3
'''
just run https://deepseek.com .
'''
import requests
import json
import sys
url = "https://api.deepseek.com/chat/completions"
headers = {
    "Content-Type": "application/json",
    "Authorization": "Bearer sk-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
}

def ask_deepseek_ai(prompt):
    data = {
        "model": "deepseek-chat",
        "messages": [
            {"role": "system", "content": "You are a helpful assistant.用中文回答"},
            {"role": "user", "content": prompt}
        ],
        "stream": True
    }
    response = requests.post(url, headers=headers, data=json.dumps(data), stream=True)
    try:
        for line in response.iter_lines():
            if line:
                decoded_line = line.decode('utf-8').lstrip("data: ")
                try:
                    print(json.loads(decoded_line)['choices'][0]['delta']['content'],end='')
                    sys.stdout.flush()
                except:
                    print('\n')
                    # if decoded_line == '[DONE]':
                    #     print(f'\n{decoded_line}')
    finally:
        response.close()
if __name__ == "__main__":
    prompt = ' '.join(sys.argv[1:])
    print('Question:',prompt)
    ask_deepseek_ai(prompt)