import os
import shutil
import subprocess
from orpsoc import utils
from .simulator import Simulator

class Verilator(Simulator):

    def __init__(self, system):
        super(Verilator, self).__init__(system)

        self.verilator_options = []
        self.src_files = []
        self.include_files = []
        self.include_dirs = []
        self.tb_toplevel = ""
	self.src_type = 'C'
	self.define_file = []

        if system.verilator is not None:
            self._load_dict(system.verilator)
        self.verilator_root = os.getenv('VERILATOR_ROOT')
        if not self.verilator_root:
            print("Environment variable VERILATOR_ROOT was not found. It should be set to the verilator install path")
            exit(1)
        self.sim_root = os.path.join(self.build_root, 'sim-verilator')

    def _load_dict(self, items):
        for item in items:
            if item == 'verilator_options':
                self.verilator_options = items.get(item).split()
            elif item == 'src_files':
                self.src_files = items.get(item).split()
            elif item == 'include_files':
                self.include_files = items.get(item).split()
                self.include_dirs  = list(set(map(os.path.dirname, self.include_files)))              
            elif item == 'tb_toplevel':
                self.tb_toplevel = items.get(item)
	    elif item == 'source_type':
		self.src_type = items.get(item)
	    elif item == 'define_file':
		self.define_file = items.get(item)
            else:
                print("Warning: Unknown item '" + item +"' in verilator section")

    def export(self):
        src_dir = self.system.files_root
        dst_dir = self.sim_root
        src_files = list(self.src_files)
        src_files += self.include_files
        src_files += [self.tb_toplevel]
        dirs = list(set(map(os.path.dirname, src_files)))
        for d in dirs:
            if not os.path.exists(os.path.join(dst_dir, d)):
                os.makedirs(os.path.join(dst_dir, d))

        for f in src_files:
            if(os.path.exists(os.path.join(src_dir, f))):
                shutil.copyfile(os.path.join(src_dir, f), 
                                os.path.join(dst_dir, f))

    def configure(self):
        super(Verilator, self).configure()
        self.export()
        self._write_config_files()

    def _write_config_files(self):
        self.verilator_file = 'input.vc'
        f = open(os.path.join(self.sim_root,self.verilator_file),'w')

        for include_dir in self.verilog.include_dirs:
            f.write("+incdir+" + os.path.abspath(include_dir) + '\n')
	f.write("+incdir+/home/carlos/projecto/orpsoc-cores/cores/wb_bfm\n") #corrigir isto.
        for src_file in self.verilog.src_files:
            f.write(os.path.abspath(src_file) + '\n')
	f.close()
	#convert define file in verilog for C
	if self.define_file:
	    fV = open (os.path.join(self.sim_root,"../src",self.define_file),'r')
	    fC = open (os.path.join(self.sim_root,"bench/verilator/",os.path.splitext(os.path.basename(self.define_file))[0]+'.h'),'w')
	    fC.write("//File auto-converted the Verilog to C. converted by ORPSOC//\n")
	    fC.write("//source file --> " + os.path.join(self.sim_root,"../src",self.define_file)+"\n")
	    for line in fV:
		Sline=line.split('`',1)
		if len(Sline) == 1:
		   fC.write(Sline[0])
		else:
		   fC.write(Sline[0]+"#"+Sline[1])
	    fC.close
	    fV.close

        
    def build(self): #choose build for source type
        super(Verilator, self).build()
	if self.src_type == 'C':
	    self.build_C()
	elif self.src_type == 'systemC':
	    self.build_SysC()
	else:
	    print("Source type not available. 'C' or 'systemC'")
	    exit(1)

    def build_C(self):
        args = ['-c']
        args += ['-I'+s for s in self.include_dirs]
        for src_file in self.src_files:
            print("Compiling " + src_file)
            utils.launch('gcc',
                         args + [src_file],
                         cwd=self.sim_root)

        object_files = [os.path.splitext(os.path.basename(s))[0]+'.o' for s in self.src_files]

        
        try:
            cmd = os.path.join(self.verilator_root,'bin','verilator')
            subprocess.check_call(['bash', cmd,
                                   '--cc',
                                   '-f', self.verilator_file,
                                   '--top-module', 'orpsoc_top',
                                   '--exe'] + 
                                  [os.path.join(self.sim_root, s) for s in object_files] + [self.tb_toplevel] + self.verilator_options,
                                  stderr = open(os.path.join(self.sim_root,'verilator.log'),'w'),
                                  cwd = os.path.join(self.sim_root))
        except OSError:
            print("Error: Command verilator not found. Make sure it is in $PATH")
            exit(1)
        except subprocess.CalledProcessError:
            print("Error: Failed to compile. See " + os.path.join(self.sim_root,'verilator.log') + " for details")
            exit(1)
        utils.launch('make -f Vorpsoc_top.mk Vorpsoc_top',
                     cwd=os.path.join(self.sim_root, 'obj_dir'),
                     shell=True)

    def build_SysC(self):

	object_files = [os.path.splitext(os.path.basename(s))[0]+'.o' for s in self.src_files]

	#verilogue
	try:
	    cmd = os.path.join(self.verilator_root,'bin','verilator') 
	    subprocess.check_call(['bash',cmd,'--sc','--top-module', 'orpsoc_top','-f',self.verilator_file,'--exe']+
				    [os.path.join(self.sim_root, s) for s in object_files]+[self.tb_toplevel]+self.verilator_options,
            			    cwd = os.path.join(self.sim_root),
	    			    stderr = open(os.path.join(self.sim_root,'verilator.log'),'w'))
        except OSError:
            print("Error: Command verilator not found. Make sure it is in $PATH")
            exit(1)
        except subprocess.CalledProcessError:
            print("Error: Failed to compile. See " + os.path.join(self.sim_root,'verilator.log') + " for details")
            exit(1)

	 #src_files	
	args = ['-I.']
	args += ['-MMD']
	args += ['-I'+s for s in self.include_dirs]
	args += ['-Iobj_dir']
	args += ['-I'+os.path.join(self.verilator_root,'include')]
	args += ['-I'+os.path.join(self.verilator_root,'include', 'vltstd')]  
	args += ['-DVL_PRINTF=printf']
	args += ['-DVM_TRACE=1']
	args += ['-DVM_COVERAGE=0']
	args += [os.getenv('SYSTEMC_CXX_FLAGS')]
	args += ['-I'+os.getenv('SYSTEMC_INCLUDE')]
	args += ['-Wno-deprecated']
	args += [os.getenv('SYSTEMC_CXX_FLAGS')]
	args += ['-c']

	for src_file in self.src_files:
	    print("Compiling " + src_file)
	    utils.launch('g++',args + ['-o' + os.path.splitext(os.path.basename(src_file))[0]+'.o']+ [src_file],
				cwd=self.sim_root)

	#tb_toplevel
        utils.launch('make -f Vorpsoc_top.mk Vorpsoc_top',
                     cwd=os.path.join(self.sim_root, 'obj_dir'),
                     shell=True)

        
    def run(self, args):
        #TODO: Handle arguments parsing
	print ("./Vorpsoc_top")
        utils.launch('./Vorpsoc_top',
                     args,
                     cwd=os.path.join(self.sim_root, 'obj_dir'))
        
