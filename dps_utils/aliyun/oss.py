#!/usr/bin/env python
# -*- coding: utf-8 -*-

import os
import oss2
import filelock
from zipfile import ZipFile
from threading import Thread

from dps_utils.misc import now


class OSS(object):
    downloaded_files = set()

    def __init__(self, access_id, access_secret, bucket, endpoint='cn-hangzhou', network='public'):
        if network == 'internal':
            endpoint += '-internal'
        endpoint = 'oss-%s.aliyuncs.com' % endpoint
        if network == 'vpc':
            endpoint = 'vpc100-' + endpoint

        self.auth = oss2.Auth(access_id, access_secret)
        self.bucket = oss2.Bucket(self.auth, endpoint, bucket)

    def list_and_download_objects(self, prefix, target, subdir=True):
        if not os.path.isdir(target):
            os.makedirs(target, 0o755)

        is_truncated = True
        next_marker = ''

        while is_truncated:
            objects = self.bucket.list_objects(prefix, '/', next_marker)
            for i in objects.object_list:
                if i.key.endswith('/'):
                    continue

                filename = os.path.join(target, i.key.rsplit('/', 1)[1])
                if os.path.isfile(filename):
                    meta = self.bucket.head_object(i.key).headers
                    if int(meta['Content-Length']) == os.path.getsize(filename):
                        with open(filename, 'rb') as f:
                            content = f.read()
                        if 'Content-MD5' in meta:
                            if meta['Content-MD5'] == oss2.utils.content_md5(content):
                                continue
                        else:
                            crc64 = oss2.utils.Crc64()
                            crc64.update(content)
                            if meta['X-Oss-Hash-Crc64ecma'] == str(crc64.crc):
                                continue

                self.get_object(i.key, filename)
                self.downloaded_files.add(filename)

            if subdir:
                for i in objects.prefix_list:
                    self.list_and_download_objects(i, os.path.join(target, i.rsplit('/', 2)[1]), subdir)

            is_truncated = objects.is_truncated
            next_marker = objects.next_marker

    def download_model_from_oss(self, name, type_, target, prefix=''):
        prefix = prefix + '/'.join([type_, name, ''])
        target = os.path.join(target, name)
        locker = filelock.FileLock(os.path.join(target, '.lock'), 1200)

        try:
            os.makedirs(target, 0o755)
        except OSError:
            pass

        with locker:
            self.downloaded_files = set()
            self.list_and_download_objects(prefix, target)

            for i in os.listdir(target):
                _file = os.path.join(target, i)
                _dir = os.path.join(target, i[:-4])

                if not (i.endswith('.zip') and _file in self.downloaded_files):
                    continue

                with ZipFile(_file) as z:
                    for j in z.namelist():
                        z.extract(j, _dir)

    def put_object(self, key, data=None, filename=None):
        if filename is None:
            return self.bucket.put_object(key, data)
        return self.bucket.put_object_from_file(key, filename)

    def get_object(self, key, filename=None):
        if filename is None:
            return self.bucket.get_object(key).read()
        return self.bucket.get_object_to_file(key, filename)

    def upload_query_image(self, data, *args, **kwargs):
        if hasattr(data, 'read'):
            data.seek(0)
            data = data.read()

        async = kwargs.pop('async', False)
        key = generate_query_key(*args, **kwargs)

        if async:
            thread = Thread(target=self.put_object, args=(key, data))
            thread.daemon = True
            thread.start()
        else:
            self.put_object(key, data)

        return key

    def upload_query_image_by_filepath(self, filepath, *args, **kwargs):
        with open(filepath, 'rb') as f:
            return self.upload_query_image(f, *args, **kwargs)


def generate_query_key(request_id, creator_id, service_id, suffix='jpg', public=True):
    return '{prefix}/{datetime}_{request_id}.{suffix}'.format(
        prefix=generate_query_prefix(creator_id, service_id, public),
        datetime=generate_query_datetime(),
        request_id=request_id,
        suffix=suffix
    )


def generate_query_prefix(creator_id, service_id, public):
    return 'query/{0}/{1}/{2}'.format(
        creator_id,
        'public' if public else 'search',
        service_id
    )


def generate_query_datetime():
    _now = now()
    return '%04d_%04d/%02d_%02d/%02d_%02d/%02d%02d%02d_%06d' % (
        9999 - _now.year, _now.year,
        12 - _now.month, _now.month,
        31 - _now.day, _now.day,
        23 - _now.hour,
        59 - _now.minute,
        59 - _now.second,
        _now.microsecond
    )
