all:
	docker build -t mmdb-release .
	docker run --rm mmdb-release > mmdb-age.gz
