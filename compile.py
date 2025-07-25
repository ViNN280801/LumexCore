#!/usr/bin/env python3
"""
Cross-platform CMake build system wrapper with colored logging.

This script provides a clean interface to build CMake projects with various
configuration options while handling errors gracefully.
"""

from os import remove as os_remove
from os import makedirs as os_makedirs
from os.path import join as os_path_join
from os.path import pathsep as os_pathsep
from os.path import isdir as os_path_isdir
from os.path import exists as os_path_exists
from os.path import abspath as os_path_abspath
from os.path import dirname as os_path_dirname

from sys import exit as sys_exit
from shutil import rmtree as shutil_rmtree
from psutil import cpu_count as psutil_cpu_count
from psutil import virtual_memory as psutil_virtual_memory

from logging import INFO as logging_INFO
from argparse import ArgumentParser as argparse_ArgumentParser

from platform import system as platform_system
from platform import machine as platform_machine

from colorlog import getLogger as colorlog_getLogger
from colorlog import StreamHandler as colorlog_StreamHandler
from colorlog import ColoredFormatter as colorlog_ColoredFormatter

from subprocess import run as subprocess_run
from subprocess import CalledProcessError as subprocess_CalledProcessError

from typing import List, Optional


class ArchitectureDetector:
    """Detects the architecture of the system."""

    @staticmethod
    def detect() -> str:
        """
        Static method to detect the architecture of the system.

        Returns:
            str: "x64" for 64-bit systems, "x86" for 32-bit systems.
        Raises:
            ValueError: If the architecture is not supported.
        """
        try:
            machine = platform_machine().lower()
            if machine in ("amd64", "x86_64", "x64"):
                return "x64"
            elif machine in ("i386", "i686"):
                return "x86"
            else:
                raise ValueError("Unsupported architecture: {}".format(machine))
        except Exception as e:
            raise RuntimeError("Failed to detect architecture: {}".format(e))


class CMakeBuilder:
    kdefault_min_cpp_standard = 11
    kdefault_max_cpp_standard = 23

    def __init__(self, project_root: str):
        """
        Initialize the builder with project root directory.

        Args:
            project_root: Path to the project root directory containing CMakeLists.txt
        """
        self.project_root = os_path_abspath(project_root)
        self.build_dir = os_path_join(self.project_root, "build")
        self.cmake_args = []
        self.architecture = ArchitectureDetector.detect()

        # Configure colorlog
        self.logger = colorlog_getLogger("CMakeBuilder")
        self.logger.setLevel(logging_INFO)

        handler = colorlog_StreamHandler()
        handler.setFormatter(
            colorlog_ColoredFormatter(
                "%(log_color)s%(levelname)-8s%(reset)s %(message)s",
                log_colors={
                    "DEBUG": "cyan",
                    "INFO": "green",
                    "WARNING": "yellow",
                    "ERROR": "red",
                    "CRITICAL": "red,bg_white",
                },
            )
        )
        self.logger.addHandler(handler)

    def run_command(self, cmd_list: List[str], cwd: Optional[str] = None) -> bool:
        """
        Execute a shell command with error handling.

        Args:
            cmd_list: List of command and arguments
            cwd: Working directory for command execution

        Returns:
            bool: True if command succeeded, False otherwise
        """
        self.logger.info("Running command: {}".format(" ".join(cmd_list)))
        try:
            subprocess_run(
                cmd_list, cwd=cwd, check=True, text=True, capture_output=False
            )
            self.logger.info("Command finished successfully.")
            return True
        except subprocess_CalledProcessError as e:
            self.logger.error(
                "Command failed with error code {}: {}".format(
                    e.returncode, " ".join(cmd_list)
                )
            )
            return False
        except FileNotFoundError:
            self.logger.error(
                "Command not found: {}. Is it installed and in PATH?".format(
                    cmd_list[0]
                )
            )
            return False
        except Exception as e:
            self.logger.critical("An unexpected error occurred: {}".format(e))
            return False

    def clean_build_dir(self) -> bool:
        """
        Description:
            Remove existing build directory.

        Returns:
            bool: True if cleaning succeeded or wasn't needed
        """
        if os_path_exists(self.build_dir):
            self.logger.info("Cleaning directory: {}".format(self.build_dir))
            try:
                shutil_rmtree(self.build_dir)
                self.logger.info("Cleaned successfully.")
                return True
            except OSError as e:
                self.logger.error(
                    "Error cleaning directory {}: {}".format(self.build_dir, e)
                )
                return False
        self.logger.info(
            "Directory {} does not exist, no need to clean.".format(self.build_dir)
        )
        return True

    def add_toolset(self, toolset: str) -> None:
        """
        Description:
            Adds a MSVC toolset to the CMake arguments.
            Supported toolsets: v141, v142, v143.
            v141 = Visual Studio 2017 (oldest supported version).
            v142 = Visual Studio 2019 (better compatibility with old Windows).
            v143 = Visual Studio 2022 (default).
            For UNIX systems, the toolset is not supported, so this function does nothing.

        Args:
            toolset (str): The MSVC toolset to add.
        """

        if platform_system() == "Windows":
            if toolset not in ["v141", "v142", "v143"]:
                self.logger.warning("Unsupported MSVC toolset: {}".format(toolset))
                return

            self.cmake_args.append("-T")
            if toolset == "v141":
                self.cmake_args.append("v141,host={}".format(self.architecture))
            elif toolset == "v142":
                self.cmake_args.append("v142,host={}".format(self.architecture))
            else:
                self.cmake_args.append("v143,host={}".format(self.architecture))
            self.logger.info("🔍 Using MSVC toolset: {}".format(toolset))

    def add_architecture(self, architecture: str) -> None:
        """
        Description:
            Adds an architecture to the CMake arguments.
            Supported architectures: x86, x64.
        """
        if architecture not in ["x86", "x64"]:
            self.logger.warning("Unsupported architecture: {}".format(architecture))
            return
        self.cmake_args.append("-DARCHITECTURE={}".format(architecture))
        self.logger.info("🔍 Using architecture: {}".format(architecture))

    def add_cpp_standard(self, cpp_standard: int, override: bool = False) -> None:
        """
        Description:
            Adds a C++ standard to the CMake arguments.
            Supported standards: 11, 14, 17, 20, 23.
            C++11 = C++11.
            C++14 = C++14.
            C++17 = C++17.
            C++20 = C++20.
            C++23 = C++23.
        """
        if cpp_standard not in range(
            self.kdefault_min_cpp_standard, self.kdefault_max_cpp_standard + 1
        ):
            self.logger.warning("Unsupported C++ standard: {}".format(cpp_standard))
            return

        current_cpp_standard_arg = None
        for arg in self.cmake_args:
            if arg.startswith("-DCMAKE_CXX_STANDARD="):
                current_cpp_standard_arg = arg
                break

        if current_cpp_standard_arg is None:
            self.cmake_args.append("-DCMAKE_CXX_STANDARD={}".format(cpp_standard))
        else:
            # Extract the current standard value from the argument string
            current_set_standard = current_cpp_standard_arg.split("=")[1]
            self.logger.warning(
                "C++ standard already set to C++{}".format(current_set_standard)
            )

            if override:
                self.logger.warning(
                    "Overriding C++ standard to C++{}".format(cpp_standard)
                )
                self.cmake_args.remove(current_cpp_standard_arg)
                self.cmake_args.append("-DCMAKE_CXX_STANDARD={}".format(cpp_standard))

    def configure(self, build_type: str) -> bool:
        """
        Configure CMake project.

        Args:
            build_type: Build type (Debug/Release)

        Returns:
            bool: True if configuration succeeded
        """
        os_makedirs(self.build_dir, exist_ok=True)

        cmake_configure_cmd = [
            "cmake",
            self.project_root,
            "-DCMAKE_BUILD_TYPE={}".format(build_type),
            "-B",
            self.build_dir,
        ]

        if self.cmake_args:
            filtered_cmake_args = [
                arg
                for arg in self.cmake_args
                if not arg.startswith("-DCMAKE_BUILD_TYPE")
            ]
            cmake_configure_cmd.extend(filtered_cmake_args)

        return self.run_command(cmake_configure_cmd)

    def add_cmake_prefix_path(self, prefix_paths: List[str]) -> None:
        """
        Description:
            Adds prefix paths to the CMake arguments.
            Multiple paths will be joined with the platform-specific path separator.

        Args:
            prefix_paths: List of prefix paths to add.

        Examples:
            ["/opt/Qt5.15.17", "F:\\local\\Qt\\6.5.3\\msvc2019_64\\lib\\cmake\\Qt6"]
        """
        if not prefix_paths:
            return

        new_prefix_string = os_pathsep.join(prefix_paths)

        found = False
        for i, arg in enumerate(self.cmake_args):
            if arg.startswith("-DCMAKE_PREFIX_PATH="):
                # If CMAKE_PREFIX_PATH already exists, append to it
                existing_paths = arg.split("=", 1)[
                    1
                ]  # Use 1 to split only on the first '='
                self.cmake_args[i] = "-DCMAKE_PREFIX_PATH={}{}{}".format(
                    existing_paths, os_pathsep, new_prefix_string
                )
                self.logger.info(
                    "Updated CMAKE_PREFIX_PATH to: {}".format(self.cmake_args[i])
                )
                found = True
                break

        if not found:
            self.cmake_args.append("-DCMAKE_PREFIX_PATH={}".format(new_prefix_string))
            self.logger.info("Added CMAKE_PREFIX_PATH: {}".format(new_prefix_string))

    def add_cmake_args(self, cmake_args: List[str]) -> None:
        """
        Description:
            Adds additional CMake arguments to the builder.

        Args:
            cmake_args: List of additional CMake arguments to add.

        Examples:
            ["-DCMAKE_PREFIX_PATH=/path/to/qt", "-DCMAKE_INSTALL_PREFIX=/path/to/install"]
        """
        self.cmake_args.extend(cmake_args)

    def activate_shared_libs(self) -> None:
        """
        Description:
            Activates shared libraries.
            Adds -DBUILD_SHARED_LIBS=ON to CMake arguments.
        """
        self.cmake_args.append("-DBUILD_SHARED_LIBS=ON")
        self.logger.info("Activated shared libraries by adding -DBUILD_SHARED_LIBS=ON.")

    def build(self, build_type: str) -> bool:
        """
        Build the configured project with dynamic parallel job count based on system resources.

        Args:
            build_type: Build type (Debug/Release)

        Returns:
            bool: True if build succeeded
        """
        # Get available CPU cores and free memory
        cpu_cores = psutil_cpu_count(logical=False)  # Physical cores
        free_memory = psutil_virtual_memory().available / (1024**3)  # Free memory in GB

        # Calculate safe parallel jobs
        if free_memory < 2.0:  # Less than 2GB free memory
            parallel_jobs = 1
        elif free_memory < 4.0:  # Less than 4GB free memory
            parallel_jobs = max(1, cpu_cores // 2)
        else:
            parallel_jobs = cpu_cores

        cmake_build_cmd = [
            "cmake",
            "--build",
            self.build_dir,
            "--config",
            build_type,
            "--parallel",
            str(parallel_jobs),
        ]
        return self.run_command(cmake_build_cmd)

    def cpack(self, build_type: str) -> bool:
        """
        Description:
            Generates an installer package using CPack.

        Args:
            build_type: Build type (Debug/Release/RelWithDebInfo)

        Returns:
            bool: True if installer generation succeeded, False otherwise
        """
        cpack_cmd = ["cpack", "-G", "NSIS", "-C", build_type]
        return self.run_command(cpack_cmd, cwd=self.build_dir)

    def install(self, install_prefix: str) -> bool:
        """
        Description:
            Installs the built project to the specified prefix.

        Args:
            install_prefix: Path to install the project to

        Returns:
            bool: True if installation succeeded, False otherwise
        """
        install_cmd = ["cmake", "--install", self.build_dir, "--prefix", install_prefix]
        return self.run_command(install_cmd)

    def dump_cmake_variables(self) -> bool:
        """
        Description:
            Dumps all CMake constants that can be passed to CMakeLists.txt.

        Returns:
            bool: True if dumping succeeded, False otherwise
        """
        from glob import glob as glob_glob
        from tempfile import mkdtemp as tempfile_mkdtemp

        temp_build_dir = None
        generated_files = []

        try:
            # Record existing files before CMake generation
            existing_files = set()
            for pattern in ["*.cmake", "CMakeCache.txt", "CMakeFiles"]:
                existing_files.update(
                    glob_glob(os_path_join(self.project_root, pattern))
                )

            # Create a temporary build directory
            temp_build_dir = tempfile_mkdtemp(prefix="cmake_dump_")

            # Configure the project to populate cache
            configure_cmd = ["cmake", self.project_root, "-B", temp_build_dir]
            configure_result = subprocess_run(
                configure_cmd, check=True, text=True, capture_output=True
            )

            if not configure_result.returncode == 0:
                self.logger.error(
                    "Error configuring the project: {}".format(configure_result.stderr)
                )
                return False

            # Now dump the cache variables
            dump_cmd = ["cmake", "-LA", temp_build_dir]
            dump_result = subprocess_run(
                dump_cmd, check=True, text=True, capture_output=False
            )

            if not dump_result.returncode == 0:
                self.logger.error(
                    "Error dumping CMake constants: {}".format(dump_result.stderr)
                )
                return False

            # Record files that were generated after CMake run
            current_files = set()
            for pattern in ["*.cmake", "CMakeCache.txt", "CMakeFiles"]:
                current_files.update(
                    glob_glob(os_path_join(self.project_root, pattern))
                )

            generated_files = list(current_files - existing_files)

            self.logger.info("CMake constants dumped successfully.")
            return True

        except subprocess_CalledProcessError as e:
            self.logger.error(
                "Error dumping CMake constants: {}".format(
                    e.stderr if e.stderr else e.stdout
                )
            )
            return False
        except FileNotFoundError:
            self.logger.error("CMake not found. Is it installed and in PATH?")
            return False
        except Exception as e:
            self.logger.critical(
                "An unexpected error occurred during dumping: {}".format(e)
            )
            return False
        finally:
            # Clean up temporary directory
            if temp_build_dir:
                try:
                    shutil_rmtree(temp_build_dir)
                except OSError:
                    pass  # Ignore cleanup errors

            # Clean up generated CMake files in project root
            for file_path in generated_files:
                try:
                    if os_path_exists(file_path):
                        if os_path_isdir(file_path):
                            shutil_rmtree(file_path)
                            self.logger.info(
                                "Removed generated directory: {}".format(file_path)
                            )
                        else:
                            os_remove(file_path)
                            self.logger.info(
                                "Removed generated file: {}".format(file_path)
                            )
                except OSError as e:
                    self.logger.warning("Failed to remove {}: {}".format(file_path, e))


class CMakeBuilderCLI:
    def __init__(self, project_root: str):
        self.project_root = project_root
        self.logger = colorlog_getLogger("CMakeBuilderCLI")
        self.logger.setLevel(logging_INFO)

        handler = colorlog_StreamHandler()
        handler.setFormatter(
            colorlog_ColoredFormatter(
                "%(log_color)s%(levelname)-8s%(reset)s %(message)s",
                log_colors={
                    "DEBUG": "cyan",
                    "INFO": "green",
                    "WARNING": "yellow",
                    "ERROR": "red",
                    "CRITICAL": "red,bg_white",
                },
            )
        )
        self.logger.addHandler(handler)

        self.parser = argparse_ArgumentParser(
            description="Cross-platform CMake build system wrapper."
        )
        self.parser.add_argument(
            "build_type",
            choices=["Debug", "Release", "RelWithDebInfo"],
            help="Build type (Debug, Release, or RelWithDebInfo)",
        )
        self.parser.add_argument(
            "-m",
            "--arch",
            choices=["x86", "x64"],
            help=(
                "Architecture to build for (x86 or x64).\n"
                "If not specified, the script will detect the architecture automatically."
            ),
        )
        self.parser.add_argument(
            "--std",
            type=int,
            default=11,  # Default C++ standard is C++11 if not specified
            help="C++ standard to use (11, 14, 17, 20, 23). Defaults to 11.",
        )
        self.parser.add_argument(
            "-r",
            "--clean",
            action="store_true",
            help="Clean the build directory before building.",
        )
        self.parser.add_argument(
            "-i",
            "--setup-installer",
            action="store_true",
            help="Build in Release mode and generate NSIS installer",
        )
        self.parser.add_argument(
            "-s",
            "--shared-libs",
            action="store_true",
            help="Build shared libraries. Adds -DBUILD_SHARED_LIBS=ON to CMake arguments.",
        )
        self.parser.add_argument(
            "--qt-path",
            help=(
                "Specify path to Qt installation (e.g., /opt/Qt5.15.17, "
                '"F:\\local\\Qt\\6.5.3\\msvc2019_64\\lib\\cmake\\Qt6") to avoid ABI conflicts.'
                "Passes CMAKE_PREFIX_PATH to CMake configuration."
            ),
        )
        self.parser.add_argument(
            "--tests",
            nargs="?",  # Allows 0 or 1 argument
            help="Enable building tests. Optionally provide a custom CMake variable name",
        )
        self.parser.add_argument(
            "--examples",
            nargs="?",
            help="Enable building examples. Optionally provide a custom CMake variable name",
        )
        self.parser.add_argument(
            "--documentation",
            nargs="?",
            help="Enable building documentation. Optionally provide a custom CMake variable name",
        )
        self.parser.add_argument(
            "--show-constants",
            action="store_true",
            help="Show all CMake constants that can be passed to CMakeLists.txt.",
        )

        # MSVC toolset selection (Windows only)
        if platform_system() == "Windows":
            self.parser.add_argument(
                "--toolset",
                choices=["v141", "v142", "v143"],
                help=(
                    "Select MSVC toolset version.\n"
                    "v141 = Visual Studio 2017 (oldest supported version).\n"
                    "v142 = Visual Studio 2019 (better compatibility with old Windows).\n"
                    "v143 = Visual Studio 2022 (default)."
                ),
            )

        self.args = self.parser.parse_args()

        if self.args.setup_installer:
            self.args.build_type = "Release"

        # Detect architecture automatically
        try:
            self.auto_detected_arch = ArchitectureDetector.detect()
        except RuntimeError as e:
            self.logger.error(str(e))
            sys_exit(1)

        self.builder = CMakeBuilder(self.project_root)

    def run(self) -> None:
        if self.args.show_constants:
            if self.builder.dump_cmake_variables():
                sys_exit(0)
            else:
                self.logger.error("ERROR: Failed to dump CMake variables.")
                sys_exit(1)

        if self.args.clean and not self.builder.clean_build_dir():
            self.logger.error("ERROR: Failed to clean build directory")
            sys_exit(1)

        # Apply C++ standard
        self.builder.add_cpp_standard(self.args.std, override=True)
        self.logger.info("⚙️ Using C++ Standard: C++{}".format(self.args.std))

        # Apply architecture
        if self.args.arch:
            self.logger.info("🔍 Adding architecture: {}".format(self.args.arch))
            self.builder.add_architecture(self.args.arch)
        else:
            self.logger.info(
                "🔍 Adding architecture: {}".format(self.auto_detected_arch)
            )
            self.builder.add_architecture(self.auto_detected_arch)

        # Apply MSVC toolset if requested
        if (
            platform_system() == "Windows"
            and hasattr(self.args, "toolset")
            and self.args.toolset
        ):
            self.logger.info("🔍 Adding MSVC toolset: {}".format(self.args.toolset))
            self.builder.add_toolset(self.args.toolset)

        if self.args.qt_path:
            self.logger.info("🔍 Adding Qt prefix path: {}".format(self.args.qt_path))
            self.builder.add_cmake_prefix_path([self.args.qt_path])

        if self.args.shared_libs:
            self.logger.info(
                "🔍 Activating shared libraries by adding -DBUILD_SHARED_LIBS=ON."
            )
            self.builder.activate_shared_libs()

        if self.args.tests is not None:
            self.builder.add_cmake_args(["-D{}={}".format(self.args.tests, "ON")])

        if self.args.examples is not None:
            self.builder.add_cmake_args(["-D{}={}".format(self.args.examples, "ON")])

        if self.args.documentation is not None:
            self.builder.add_cmake_args(
                ["-D{}={}".format(self.args.documentation, "ON")]
            )

        if not self.builder.configure(self.args.build_type):
            self.logger.error("ERROR: CMake configuration failed")
            sys_exit(1)

        if not self.builder.build(self.args.build_type):
            self.logger.error("ERROR: Build failed")
            sys_exit(1)

        if self.args.setup_installer:
            if not self.builder.cpack(self.args.build_type):
                self.logger.error("ERROR: Installer generation failed")
                sys_exit(1)


def main():
    cli = CMakeBuilderCLI(os_path_dirname(os_path_abspath(__file__)))
    cli.run()


if __name__ == "__main__":
    try:
        main()
    except KeyboardInterrupt:
        colorlog_getLogger().info("Build process interrupted by user.")
        sys_exit(1)
    except Exception as e:
        colorlog_getLogger().critical("An unexpected error occurred: {}".format(e))
        sys_exit(1)
