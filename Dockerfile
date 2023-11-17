FROM archlinux

# Update everything
RUN pacman -Syu --noconfirm

# Install a C compiler, build dependencies and other useful stuff
RUN pacman -S --noconfirm base-devel cmake vim git

# Setup a new user
ARG USER=user
RUN useradd -m ${USER} && chown -R ${USER}:${USER} /home/${USER}

# Setup the working directory
WORKDIR /home/${USER}
COPY --chown=${USER} . workspace
USER ${USER}

# Clean out old build files
RUN rm -rf workspace/build

RUN mkdir -p workspace/build && cd workspace/build && cmake .. && make -j$(nproc)
