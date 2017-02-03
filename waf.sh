#!/usr/bin/env bash

command python --version >/dev/null 2>&1 || 
{ 
	echo >&2 "The python interpreter cannot be found. Aborting."; 
	exit 1; 
}

python ./tools/thirdparty/waf/waf $@