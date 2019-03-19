# start with the node image
FROM alpine:3.9 as builder

RUN set -ex \
    && apk update \
    && apk add \
        autoconf \
        pkgconf \
        curl \
        automake \
        libtool \
        make \
        g++ \
        build-base \
        gcc \
        zlib-dev \
        cmake \
        libcurl \
        mosquitto-clients mosquitto-dev && \
    rm -f /var/cache/apk/*


RUN mkdir -p /usr/local/include/nlohmann
COPY ./src/nlohmann/json.hpp /usr/local/include/nlohmann/json.hpp

ENV LANG C.UTF-8

# Create app directory in the image
WORKDIR /usr/src/predix-edge-sample-scaler-cpp

# copy app's source files to the image

COPY src ./src
RUN make -f ./src/Makefile

FROM alpine:3.9

LABEL maintainer="Predix Edge Application Services"
LABEL hub="https://hub.docker.com"
LABEL org="https://hub.docker.com/u/predixedge"
LABEL repo="predix-edge-sample-scaler-cpp"
LABEL version="1.0.2"
LABEL support="https://forum.predix.io"
LABEL license="https://github.com/PredixDev/predix-docker-samples/blob/master/LICENSE.md"

RUN apk add --no-cache mosquitto-clients mosquitto-dev
WORKDIR /usr/src/predix-edge-sample-scaler-cpp

COPY --from=builder /usr/src/predix-edge-sample-scaler-cpp/src/scaler_app ./scaler_app
COPY --from=builder /usr/local/include/nlohmann/json.hpp /usr/local/include/nlohmann/json.hpp

CMD ["./scaler_app"]
