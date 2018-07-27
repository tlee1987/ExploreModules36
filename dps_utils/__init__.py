#!/usr/bin/env python
# -*- coding: utf-8 -*-

import logging


logger = logging.getLogger('dps_utils')
logger.setLevel(logging.INFO)
handler = logging.StreamHandler()
handler.setLevel(logging.INFO)
logger.addHandler(handler)


__version__ = '0.4.3'
