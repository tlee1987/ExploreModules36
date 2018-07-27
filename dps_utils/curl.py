#!/usr/bin/env python
# -*- coding: utf-8 -*-

import requests

from dps_utils.misc import retry


def assert_ok(r, raw=False):
    if not isinstance(r, requests.models.Response):
        raise TypeError('Expected <requests.models.Response>, <%s> provided.' % type(r))

    if not r.ok:
        raise requests.HTTPError('HTTP error. URL: %s. Status code: %d. Body:\n%s' % (
            r.url, r.status_code, r.text), response=r)

    return r.content if raw else r.text


def json_decode(r):
    try:
        response = r.json()
    except ValueError:
        assert_ok(r)
        raise ValueError('JSON decode error. URL: %s. Body:\n%s' % (r.url, r.text))

    if isinstance(response, dict) and response.get('error_code', response.get('is_err')):
        raise requests.HTTPError('API got an error. Message: %s' % response.get(
            'message', response.get('msg', 'Unknown Error')), response=r)

    assert_ok(r)
    return response


@retry(2, 'Request failed.', False)
def request(url, method='POST', timeout=(2, 8), **kwargs):
    decode_json = kwargs.pop('decode_json', True)
    r = requests.request(method, url, timeout=timeout, **kwargs)
    if decode_json:
        return json_decode(r)
    return assert_ok(r)


@retry(3, 'Download failed.', False)
def download(url, timeout=(3, 10)):
    r = requests.get(url, timeout=timeout)
    return assert_ok(r, True)
