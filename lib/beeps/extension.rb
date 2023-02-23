# -*- coding: utf-8 -*-


module Beeps


  module Module

    module_function

    def name()
      super.split('::')[-2]
    end

    def version()
      open(root_dir 'VERSION') {|f| f.readline.chomp}
    end

    def root_dir(path = '')
      File.expand_path "../../#{path}", __dir__
    end

    def inc_dir()
      root_dir 'include'
    end

    def lib_dir()
      root_dir 'lib'
    end

  end# Module


end# Beeps
