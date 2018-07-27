#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import json
from six import iteritems, text_type
from hashlib import sha1 as _sha1
from datetime import datetime

from dps_utils import logger


def sha1(data):
    if not isinstance(data, bytes):
        data = data.encode('utf-8')
    return _sha1(data).hexdigest()


def now():
    return datetime.utcnow()


def update_config_from_env(config, prefix=None):
    if prefix is not None:
        prefix = prefix.upper() + '_'

    for k, v in iteritems(os.environ):
        if prefix is not None and not k.startswith(prefix):
            continue

        k = k.split('_', 1)[1]
        v = v.strip()
        if k not in config:
            continue

        if isinstance(config[k], (dict, list)):
            v = json.loads(v)
        elif isinstance(config[k], int):
            v = int(v)
        else:
            try:
                v = v.decode('utf-8')
            except (UnicodeDecodeError, AttributeError):
                pass

        if isinstance(config[k], dict):
            config[k].update(v)
        else:
            config[k] = v


def retry(times, prefix=None, silent=True):
    def wrapper(func):
        def do_retry(*args, **kwargs):
            _times = kwargs.pop('max_retries', times)
            _silent = kwargs.pop('silent', silent)

            if _times is not None:
                _times = int(_times)
                if _times <= 0:
                    raise ValueError('times must be greater than 0.')

            i = 0
            while True:
                i += 1

                try:
                    return func(*args, **kwargs)
                except Exception as e:
                    logger.info('Retrying: %d%s. %s%s' % (
                        i,
                        '' if _times is None else '/%d' % _times,
                        '' if prefix is None else '%s ' % prefix,
                        text_type(e),
                    ))
                    if _times is not None and _times <= i:
                        if _silent:
                            return e
                        raise e

        return do_retry
    return wrapper
