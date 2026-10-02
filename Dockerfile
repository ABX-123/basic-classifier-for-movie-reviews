FROM python:3.12-slim

# ==============================================================================
# Native build dependencies
# ==============================================================================
RUN apt-get update && apt-get install -y --no-install-recommends \
    build-essential \
    cmake \
    wget \
    ca-certificates \
    tar \
    libgtest-dev \
    && rm -rf /var/lib/apt/lists/*

# ==============================================================================
# Python dependencies
# ==============================================================================
RUN pip install --no-cache-dir \
    scikit-learn

# ==============================================================================
# IMDb dataset
# ==============================================================================
WORKDIR /app

RUN mkdir -p /app/data && \
    wget -q https://ai.stanford.edu/~amaas/data/sentiment/aclImdb_v1.tar.gz -O /tmp/aclImdb.tar.gz && \
    tar -xzf /tmp/aclImdb.tar.gz -C /app/data && \
    rm /tmp/aclImdb.tar.gz

# ==============================================================================
# Application source
# ==============================================================================
COPY CMakeLists.txt .
COPY src ./src
COPY python ./python
COPY tests ./tests

# ==============================================================================
# Train once while building the image.
# ==============================================================================
RUN python python/train_model.py

# ==============================================================================
# Build & Test execution during construction
# ==============================================================================
RUN cmake -S . -B build -DCMAKE_BUILD_TYPE=Release && \
    cmake --build build --parallel

#RUN ctest --test-dir build --output-on-failure

# ==============================================================================
# Run execution
# ==============================================================================
CMD ["./build/sentiment"]
