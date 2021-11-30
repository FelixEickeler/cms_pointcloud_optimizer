## CMS PointCloud Optimizer
This git contains a reimplementation of the Paper (
[Adaptive feature-conserving compression for large scale point clouds](https://www.researchgate.net/publication/348431058_Adaptive_feature-conserving_compression_for_large_scale_point_clouds#fullTextFileContent). 
It is a practical implementation of a bilateral filtering method that takes multiple point properties into account:
(Position, Normals, Colours, Intensity, ... ). In contrast to other compression approaches, this is a lossy compression (e.g. downsampling).

The main idea is to not modify the behaviour of the point cloud in engineering tasks and fixed workflows while reducing the time and computational requirements. 

### Quickstart
If you just want to test this tool, a docker reduced docker is available:
docker run cms_compressor [cmd args] 

### Commandline Parameters
After compiling, the app is supports the follwing commands:

####Compression Settings
-r --compression_ratio  
-p --points  

--update_cost  Writes the cost as scalar filed
--estimate_normals Estimate normals [knn=N where N is the NN-Count]

-c --config_path path to a given configuration file. Not existing attributes will be
taken from std_config.json [*.json]

-o --output_path
(required)  output path for the compressed point cloud [*.ply]

-i --input_path 
(required)  path to a point cloud [*.ply]

--ignore_rest
Ignores the rest of the labeled arguments following this flag.

--version
Displays version information and exits.





### Start Development Docker
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
UUID="$(id -u)" GID="$(id -g)" docker-compose run box-office
```
This command might take a significant amount of time. So be sure to grab a coffee.

To start the docker as daemon (development purpose):
```
UUID="$(id -u)" GID="$(id -g)" docker-compose up -d
docker-compose exec -u boxy box-office bash
```

You now can connect with ssh on port 2242 and the username:simpl, pw: theoldone
Do not forget to shutdown the docker after use with docker-compose down

### TODOS