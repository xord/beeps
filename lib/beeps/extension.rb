module Beeps


  module Extension

    module_function

    def name()
      super.split('::')[-2]
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

  end# Extension


end# Beeps
