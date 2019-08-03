#!/bin/bash
# Script to run before_install step on Travis-CI
#
# Version: 20190802

# Exit on error.
set -e;

if test ${TARGET} = "docker";
then
	sudo apt-get update;
	sudo apt-mark hold mysql-server-5.7;
	sudo apt-get --fix-missing -o Dpkg::Options::="--force-confold" upgrade -y --allow-unauthenticated;

	sudo apt-get install -y qemu-user-static;

	CONTAINER_NAME="testcontainer";

	docker pull ${DOCKERHUB_REPO}:${DOCKERHUB_TAG};

	docker run --name=${CONTAINER_NAME} --detach -i ${DOCKERHUB_REPO}:${DOCKERHUB_TAG};

	docker run --rm --privileged multiarch/qemu-user-static --reset -p yes;

	# Install add-apt-repository and locale-gen.
	docker exec ${CONTAINER_NAME} apt-get update -q;
	docker exec -e "DEBIAN_FRONTEND=noninteractive" ${CONTAINER_NAME} sh -c "apt-get install -y locales software-properties-common";

	# Set locale to US English and UTF-8.
	docker exec ${CONTAINER_NAME} locale-gen en_US.UTF-8;

	# Install packages essential for building.
	docker exec -e "DEBIAN_FRONTEND=noninteractive" ${CONTAINER_NAME} sh -c "apt-get install -y autoconf automake build-essential libtool pkg-config autopoint";

elif test ${TRAVIS_OS_NAME} = "linux" || test ${TRAVIS_OS_NAME} = "linux-ppc64le";
then
	sudo apt-get update;
	sudo apt-mark hold mysql-server-5.7;
	sudo apt-get --fix-missing -o Dpkg::Options::="--force-confold" upgrade -y --allow-unauthenticated;

	sudo apt-get install -y autopoint;

elif test ${TRAVIS_OS_NAME} = "osx";
then
	brew update

	brew install gettext gnu-sed;
	brew link --force gettext;
fi

if test ${TARGET} = "coverity";
then
	echo -n | openssl s_client -connect scan.coverity.com:443 | sed -ne '/-BEGIN CERTIFICATE-/,/-END CERTIFICATE-/p' | sudo tee -a /etc/ssl/certs/ca-;
fi

