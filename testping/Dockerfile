# Dockerfile
FROM ubuntu:latest

# 필요한 패키지 설치
RUN apt-get update && apt-get install -y iputils-ping

# 핑 테스트를 위해 기본적으로 대기하도록 설정
CMD ["tail", "-f", "/dev/null"]
