#!/usr/bin/env python
# -*- coding: utf-8 -*-


class DPSException(Exception):
    pass


class AliyunException(Exception):
    pass


class ModelException(DPSException):
    pass


class ProjectNotFound(ModelException):
    pass
