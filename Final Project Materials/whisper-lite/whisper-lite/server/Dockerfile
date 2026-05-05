FROM python:3.12-slim

WORKDIR /app

COPY requirements.txt ./
RUN pip install --no-cache-dir -r requirements.txt

COPY server.py ./

# Certs are mounted at runtime so they're not baked into the image.
# docker run -v $PWD/cert.pem:/app/cert.pem -v $PWD/key.pem:/app/key.pem \
#            -p 8765:8765 whisper-server

EXPOSE 8765

CMD ["python", "server.py", "--cert", "/app/cert.pem", "--key", "/app/key.pem"]
