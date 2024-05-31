#!/bin/bash
# just run https://deepseek.com .

API_KEY="sk-xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx"
API_URL="https://api.deepseek.com/chat/completions"
helpme() {
  local question=$*
  echo "Question: $question"
  local response=$(curl -s -X POST $API_URL \
    -H "Content-Type: application/json" \
    -H "Authorization: Bearer $API_KEY" \
    -d '{
     "model": "deepseek-chat",
        "messages": [
          {"role": "system", "content": "You are a helpful assistant.用中文回答"},
          {"role": "user", "content":"'"$question"'"}
        ],
        "stream": false
    }')

  local answer=$(echo $response | jq -r '.choices[0].message.content' )

  
  echo "Answer: $answer"
}

translate () {
    local question=$*
    echo "Question: $question"
    local response=$(curl -s -X POST "https://api.deepseek.com/chat/completions" 
    -H "Content-Type: application/json"     
    -H "Authorization: Bearer $API_KEY"     
    -d '{
     "model": "deepseek-chat",
        "messages": [
          {"role": "system", "content": "你是一个好用的翻译助手。请将我的中文翻译成英文，将所有非中文的翻译成中文。我发给你所有的话都是需要翻译的内容，你只需要回答翻译结果。翻译结果请符合中文的语言习惯。"},
          {"role": "user", "content":"'"$question"'"}
        ],
        "stream": false
    }');
    local answer=$(echo $response | jq -r '.choices[0].message.content' );
    echo "Answer: $answer"
}

