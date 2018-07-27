#!/usr/bin/env python
# -*- coding: utf-8 -*-

from functools import partial

from dps_utils import __version__
from dps_utils.model.project import Project
from dps_utils.model.translation import Translation, translate


class Model(object):
    endpoint = 'http://localhost:8080/'
    headers = {'User-Agent': 'dps_utils/' + __version__}

    def __init__(self, endpoint=None, auth=None, **kwargs):
        if endpoint is not None:
            if not endpoint.startswith('http'):
                endpoint = 'http://' + endpoint
            if not endpoint.endswith('/'):
                endpoint += '/'
            self.endpoint = endpoint
        self.headers.update({'auth': auth})

        if kwargs:
            _project = self.project()
            self.project_info = _project.get(kwargs['id']) \
                if 'id' in kwargs else _project.query(**kwargs)[0]

            _translation = self.translation()
            self.translate = partial(translate, translations=_translation.get(self.project_info['id']))

    @property
    def project(self):
        return partial(Project, self.endpoint, self.headers)

    @property
    def translation(self):
        return partial(Translation, self.endpoint, self.headers)
