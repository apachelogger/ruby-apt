require 'mkmf-rice'

$LIBS << ' -lapt-pkg'

create_makefile('test')
