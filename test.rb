require 'test'
t = PkgCacheFile.new
p t
t.close
p t.readOnlyOpen || raise
p t

# p t.pkgCache
# p t.pkgDepCache

c = t.pkgCache
# .findPkg("kwin-x11")
puts " i = c.findPkg 'ubuntu'"

puts " c.each { |x| p x "

# what is missing is being able to conveniently access the Packages coming out of each

require 'irb'
binding.irb
