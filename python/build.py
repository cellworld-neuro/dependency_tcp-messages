import sys
from easy_pack import EasyPackModule
from os import path

module = EasyPackModule.read('.')
if not path.exists('setup/setup.py') or path.getctime('__info__.py') > path.getctime('setup/setup.py'):
	print('package info file has changed, rebuilding setup')
	module.create_setup_files('../setup')
build = module.build_module('python-build')
if build:
	print('build succeded')
	if '-upload' in sys.argv:
		import os
		if '-user':
			username = sys.argv[sys.argv.index('-user')+1]
		if '-password':
			password = sys.argv[sys.argv.index('-password')+1]
		if '-repository':
			repository = sys.argv[sys.argv.index('-repository')+1]
		os.system('cd ' + build + '; twine upload dist/*' + ((' --repository-url  ' + repository) if username else '') + ((' -u ' + username) if username else '') + ((' -p ' + password) if password else ''))
	else:
		print('use twine upload --repository-url [pypi-repository-url] dist/* to upload the package')
	if '-install' in sys.argv:
		os.system('cd ' + build + '; pip install .')
	module.save('.')
else:
	print('build failed')