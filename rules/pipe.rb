# Copyright 2013 David Axmark
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# 	http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

require "#{File.dirname(__FILE__)}/gcc.rb"
require "#{File.dirname(__FILE__)}/mosync_util.rb"
require "#{File.dirname(__FILE__)}/targets.rb"

# load local_config.rb, if it exists.
lc = "#{File.dirname(__FILE__)}/local_config.rb"
require lc if(File.exists?(lc))

module MoSyncInclude
	def mosync_include; "#{mosyncdir}/include" + sub_include; end
	def mosync_libdir; "#{mosyncdir}/lib"; end
	def sub_include; USE_NEWLIB ? "/newlib" : ""; end
end

class PipeTask < MultiFileTask
	def initialize(work, name, objects, linkflags, files = [])
		super(work, name, files)
		@FLAGS = linkflags
		dirTask = DirTask.new(work, File.dirname(name))
		@objects = objects
		@prerequisites += @objects + [dirTask]

		initFlags
	end

	def needed?(log = true)
		return true if(super(log))
		return flagsNeeded?(log)
	end

	def cFlags
		return "#{@FLAGS} \"#{@NAME}\" \"#{@objects.join('" "')}\""
	end

	def execute
		execFlags
		# pipe-tool may output an empty file and then fail.
		begin
			sh "#{mosyncdir}/bin/pipe-tool#{cFlags}"
		rescue => e
			FileUtils.rm_f(@NAME)
			raise
		end
		if(!File.exist?(@NAME))
			error "Pipe-tool failed silently!"
		end
	end

	include FlagsChanged
end

# adds dependency handling
class PipeResourceTask < PipeTask
	def initialize(work, name, objects)
		@depFile = "#{File.dirname(name)}/resources.mf"
		@tempDepFile = "#{@depFile}t"
		super(work, name, objects, " -depend=#{@tempDepFile} -R")

		# only if the file is not already needed do we care about extra dependencies
		if(!needed?(false)) then
			@prerequisites += MakeDependLoader.load(@depFile, @NAME)
		end
	end
	def needed?(log = true)
		if(!File.exists?(@depFile))
			puts "Because the dependency file is missing:" if(log)
			return true
		end
		return super(log)
	end
	def execute
		super
		FileUtils.mv(@tempDepFile, @depFile)
	end
end

class PipeGccWork < GccWork
	def gccVersionClass; PipeGccWork; end
	include GccVersion

	def gcc
		default_const(:GCC_DRIVER_NAME, mosyncdir + "/bin/xgcc")
		return GCC_DRIVER_NAME
	end

	def gccmode; "-S"; end
	def host_flags;
		flags = ''
		flags += ' -g' #if(CONFIG != '')
		flags += ' -DUSE_NEWLIB' if(USE_NEWLIB)
		return flags
	end
	def host_cppflags
		return ''#' -frtti'
	end

	include MoSyncInclude

	def set_defaults
		default(:BUILDDIR_PREFIX, "")
		default(:COMMOM_BUILDDDIR_PREFIX, "")
		if(USE_NEWLIB)
			@BUILDDIR_PREFIX += "newlib_"
			@COMMOM_BUILDDDIR_PREFIX += "newlib_"
		else
			@BUILDDIR_PREFIX += "pipe_"
			@COMMOM_BUILDDDIR_PREFIX += "pipe_"
		end
		super
	end

	private

	def object_ending; ".s"; end

	def pipeTaskClass; PipeTask; end

	def setup3(all_objects, have_cppfiles)
		#puts all_objects
		llo = @LOCAL_LIBS.collect { |ll| FileTask.new(self, @COMMON_BUILDDIR + ll + ".lib") }
		need(:@NAME)
		if(@TARGET_PATH == nil)
			need(:@BUILDDIR)
			need(:@TARGETDIR)
			@TARGET_PATH = @TARGETDIR + "/" + @BUILDDIR + "program"
			if(ELIM)
				@TARGET_PATH += "e"
			end
		end
		@TARGET = pipeTaskClass.new(self, @TARGET_PATH, (all_objects + llo), @FLAGS + @EXTRA_LINKFLAGS)
		@prerequisites += [@TARGET]
	end
end
