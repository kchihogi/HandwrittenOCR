FROM mcr.microsoft.com/devcontainers/cpp:debian

RUN apt-get update && apt-get install -y \
    cmake \
    curl \
    tesseract-ocr \
    tesseract-ocr-jpn \
    libtesseract-dev \
    libleptonica-dev \
    libopencv-dev \
    python3 \
    python3-pip \
    python3-venv \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app
COPY . /app

RUN ln -s /usr/bin/python3 /usr/bin/python
RUN python -m venv /app/venv
RUN /bin/bash -c "source /app/venv/bin/activate && pip install --upgrade pip && pip install --no-cache-dir -r /app/backend/requirements.txt"
ENV PATH="/app/venv/bin:$PATH"

RUN cd /app/ocr_module && if [ ! -d "build" ]; then mkdir build; fi && cd build && cmake .. && make

CMD ["uvicorn", "backend.src.app:app", "--host", "0.0.0.0", "--port", "8000", "--reload"]

# RUN apt-get update && apt-get install -y \
#     software-properties-common \
#     && add-apt-repository ppa:alex-p/tesseract-ocr5

# RUN apt update && apt install -y \
#     tesseract-ocr \
#     tesseract-ocr-jpn \
#     libtesseract-dev \
#     libleptonica-dev
