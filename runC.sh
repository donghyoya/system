#!/bin/bash

# 첫 번째 매개변수로 받은 파일 이름
FILENAME=$1

# 파일 확장자 없이 파일 이름만 추출
BASENAME=$(basename "$FILENAME" .c)

# 컴파일
gcc "$FILENAME" -o "$BASENAME" -lpcap

# 실행 파일이 성공적으로 생성되었는지 확인
if [ -f "./$BASENAME" ]; then
    echo "Running $BASENAME..."
    ./"$BASENAME"
else
    echo "Compilation failed."
fi

