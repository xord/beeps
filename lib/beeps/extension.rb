module Beeps


  module Extension

    module_function

    def name(downcase = false)
      super().split('::')[-2].then {|s|
        downcase ? s.gsub(/([a-z])([A-Z])/) {"#{$1}-#{$2}"}.downcase : s
      }
    end

    def version()
      File.read(root_dir 'VERSION')[/[\d\.]+/]
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

    def ext_dir()
      root_dir 'ext'
    end

  end# Extension


end# Beeps
