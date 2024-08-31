#!/bin/bash

version="$1"
specdir="$2"

cat "${specdir}/nautilus-avinfo.spec.template" | VERSION=${version} envsubst > "${specdir}/nautilus-avinfo.spec"
