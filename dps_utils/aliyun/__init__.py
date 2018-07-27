#!/usr/bin/env python
# -*- coding: utf-8 -*-

from functools import partial

from dps_utils.aliyun.oss import OSS


class Aliyun(object):

    def __init__(self, access_id, access_secret):
        self.access_id = access_id
        self.access_secret = access_secret

    @property
    def oss(self):
        return partial(OSS, self.access_id, self.access_secret)
