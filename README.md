## CMS PointCloud Optimizer
This git contains a reimplementation of the Paper (
[Adaptive feature-conserving compression for large scale point clouds](https://www.researchgate.net/publication/348431058_Adaptive_feature-conserving_compression_for_large_scale_point_clouds#fullTextFileContent). 
It is a practical implementation of a bilateral filtering method that takes multiple point properties into account:
(Position, Normals, Colours, Intensity, ... ). In contrast to other compression approaches, this is a lossy compression (e.g. downsampling).

The main idea is to not modify the behaviour of the point cloud in engineering tasks and fixed workflows while reducing the time and computational requirements. 

## :rocket: Quickstart
If you just want to test this tool, a docker reduced docker is available:
```
docker run -v {local_input_folder}:/in -v {local_output_folder}:/out eickeler/pointcloud_optimizer -i /in/{input_filename.ply} -o /out/{output_filename.ply} -r 0.4
```
> {local_input_folder} --- local input folder of the file you want to compress. Use $(pwd) for current directory.)
> 
> {local_output_folder} --- local output folder (take care the tool will just overwrite existing data without notice)
> 
> {input_filename.ply} --- filename including the suffix. For now, only ply is supported
> 
> {output_filename} --- filename including the suffix

<br/>

### :artificial_satellite: Compression Settings
| short |     option       |    description     | default |
|:-------:|:------------------|:--------------------|:---------:|
| -i | --input_path  |  input path  [*.ply] | - |
| -o | --output_path |  output path  [*.ply] | - |
| -c | --config_path | path to a given configuration file. Non-existing attributes will be taken from doc/default.json| doc/default.json
| -r | --compression_ratio |  remaining points factor | - |
| -p | --points | remaining points count| - |
| -  | --update_cost | Writes the cost as scalar filed | false |
| -  | --estimate_normals | Estimate normals  | [knn=5]|  |
| -  | --version | Displays version information and exits. |  - |

<br />

## :whale2: Start Development Docker
A full development environment based on debian bullseye is available. After starting up the docker you will
be able to connect via port localhost:2243. This can be used by your favorite IDE (Clion).

```
git clone https://github.com/FelixEickeler/cms_pointcloud_optimizer.git
git submodule init
cd cms_pointcloud_optimizer/docker
```
Change the docker-compose.yml, volumes if you need to change mounts or want to change the ports.
To start the docker built  / run:
```
UUID="$(id -u)" GID="$(id -g)" docker-compose run cms_optimizer_dev
```
This command might take a significant amount of time. So be sure to grab a coffee.

To start the docker as daemon (development purpose):
```
UUID="$(id -u)" GID="$(id -g)" docker-compose up -d cms_optimizer_dev
docker-compose exec -u simpl cms_optimizer_dev bash
```

You now can connect with ssh on port 2243 and the username:simpl, pw: theoldone
Do not forget to shutdown the docker after use with docker-compose down

<br />

## :building_construction: Building 
Building should be straight forward:
```
git clone https://github.com/FelixEickeler/cms_pointcloud_optimizer.git && cd cms_pointcloud_optimizer 
git submodule init && git submodule update
mkdir _build && cd _build
cmake ..
make -j 8
```

## TODOS