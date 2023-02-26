def sh(cmd)
  puts cmd
  system cmd
end

def setup_dependencies(build: true, only: nil)
  gemspec_path = `git ls-files`.lines(chomp: true).find {|l| l =~ /\.gemspec$/}
  gemspec      = File.read gemspec_path
  name         = File.basename gemspec_path, '.gemspec'

  exts = File.readlines('Rakefile')
    .map {|l| l[%r|^\s*require\W+(\w+)/extension\W+$|, 1]}
    .compact
    .reject {|ext| ext == name}
  exts = exts & [only].flatten.map(&:to_s) if only

  exts.each do |ext|
    ver = gemspec[/add_runtime_dependency.*'#{ext}'.*'\s*~>\s*([\d\.]+)\s*'/, 1]
    url = "https://github.com/xord/#{ext}.git"
    sh %( git clone --depth 1 --branch v#{ver} #{url} ../#{ext} )
    sh %( cd ../#{ext} && rake ext )
  end
end

def tag_versions()
  tags = `git tag`.lines chomp: true
  vers = `git log --oneline ./VERSION`
    .lines(chomp: true)
    .map {|line| line.split.first[/^\w+$/]}
    .map {|hash| [`git cat-file -p #{hash}:./VERSION 2>/dev/null`[/[\d\.]+/], hash]}
    .select {|ver, hash| ver && hash}
    .reverse
    .to_h

  changes = File.read('ChangeLog.md')
    .split(/^\s*##\s*\[\s*v([\d\.]+)\s*\].*$/)
    .slice(1..-1)
    .each_slice(2)
    .to_h
    .transform_values(&:strip!)

  vers.to_a.reverse.each do |ver, hash|
    tag = "v#{ver}"
    break if tags.include?(tag)
    sh %( git tag -a -m \"#{changes[ver]&.gsub '"', '\\"'}\" #{tag} #{hash} )
  end
end
