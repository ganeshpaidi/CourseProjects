from django import template

register = template.Library()

@register.filter
def index(indexable, i):
    return indexable[i]


@register.filter
def index2d(indexable,i,j):
    return indexable[i][j]

@register.filter
def slice(list,n):
    return list[0:int(n)]