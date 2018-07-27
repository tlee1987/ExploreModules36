#!/usr/bin/env python
# -*- coding: utf-8 -*-

from requests import HTTPError

from dps_utils.curl import request
from dps_utils.exceptions import ProjectNotFound


class Project(object):

    def __init__(self, endpoint, headers=None):
        self.endpoint = endpoint + 'project/'
        self.headers = headers

    def get(self, id_):
        try:
            project = request(self.endpoint + str(id_), 'GET', headers=self.headers)
        except HTTPError as e:
            if e.response.status_code == 404:
                raise ProjectNotFound()
            raise e
        return project

    def query(self, **kwargs):
        projects = request(self.endpoint, 'GET', headers=self.headers, params=kwargs)
        if not projects:
            raise ProjectNotFound()
        return projects
