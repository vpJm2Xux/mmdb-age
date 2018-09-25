FROM i386/alpine

RUN apk add gcc musl-dev

WORKDIR /root
ADD mmdb-age.c /root

RUN \
	gcc -static -s -W -Wall mmdb-age.c -o mmdb-age && \
	gzip -9 mmdb-age && \
	echo OK

CMD cat mmdb-age.gz
