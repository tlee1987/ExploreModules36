#!/usr/bin/env python
# -*- coding: utf-8 -*-

from six import iteritems, string_types
from langcodes import best_match

from dps_utils.curl import request


class Translation(object):

    def __init__(self, endpoint, headers):
        self.endpoint = endpoint + 'translation/'
        self.headers = headers

    def get(self, project_id):
        # https://engine.gitpages.malong.com/pai-models-manager/#/Translation/get_translation_
        results = request(self.endpoint, 'GET', headers=self.headers, params={'project_id': project_id})
        return {
            trans_info['text']: {
                lang_code: message
                for lang_code, message in iteritems(trans_info['translations']) if message
            }
            for trans_info in results if trans_info['project_id'] == project_id
        }


def translate(message, languages, translations):
    translations = translations.get(message)
    puid = message
    if translations:
        supported_languages = set(translations.keys())
        language = best_match_language(languages, supported_languages)
        if language:
            message = translations[language]
    return message, puid


def best_match_language(languages, supported_languages):
    if callable(languages):
        languages = languages()
    if not languages:
        return
    if isinstance(languages, string_types):
        languages = [languages]

    for i in languages:
        # https://github.com/LuminosoInsight/langcodes#match-values
        language = best_match(i, supported_languages, 75)[0]
        if language != 'und':
            return language
