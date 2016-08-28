FROM resin/rpi-raspbian

MAINTAINER hfreire@exec.sh

RUN apt-get update && \
 DEBIAN_FRONTEND=noninteractive apt-get install -y \
        libglib2.0-0

COPY build/raspowmeter /usr/local/bin

CMD ["raspowmeter"]