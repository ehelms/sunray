from .base import BASE_DIR


GLOBAL_MEDIA_DIRS = (
    BASE_DIR + 'static/', 
    BASE_DIR + 'apps/',
)

MEDIA_BUNDLES = (
    ('main.css',
        'stylesheets/main.scss',
    ),
    ('visualization.css',
        'visualization/static/stylesheets/visual.scss',
    ),
)

MEDIA_BUNDLES += (
    ('common.js',
        'javascript/jquery/jquery-1.6.4.js',
        'javascript/sunray.js'
    ),
    ('visualization.js',
        'visualization/static/javascript/visual.js',
        'visualization/static/javascript/render.js',
    ),
)
