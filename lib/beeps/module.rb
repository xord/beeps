# -*- coding: utf-8 -*-


module Beeps


  module Module

    def name ()
      super.split('::')[-2]
    end

    def version ()
      open(root_dir 'VERSION') {|f| f.readline.chomp}
    end

    def root_dir (path = '')
      File.expand_path "../../../#{path}", __FILE__
    end

    def include_dir ()
      root_dir 'include'
    end

    def lib_dir ()
      root_dir 'lib'
    end

    def task_dir ()
      root_dir 'task'
    end

    def load_tasks (*names)
      if names.empty?
        Dir["#{task_dir}/**/*.rake"].each {|path| load path}
      else
        names.each do |name|
          path = "#{task_dir}/#{name}.rake"
          load path if File.exist? path
        end
      end
    end

    extend self

  end# Module


end# Beeps
