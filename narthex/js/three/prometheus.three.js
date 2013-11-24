function RobotArm()
{
	var self = this;

	self.container = $('#3d-prometheus-container');
	self.width = 1000;
	self.height = 500;

	self.meshes = new Array();

	self.init = function()
	{
		// Camera
		self.camera = new THREE.PerspectiveCamera(45, self.width / self.height, 0.1, 10000);
		self.camera.position.z = 750;
		self.camera.position.y = 0;

		// Scene
		self.scene = new THREE.Scene();

		// Renderer
		self.renderer = new THREE.WebGLRenderer();
		self.renderer.setSize(self.width, self.height);

		self.container.get(0).appendChild(self.renderer.domElement);

		// Material
		self.material =
			new THREE.MeshLambertMaterial(
			{
				color: 0xCC0000
			});

		// Light
		self.light =
			new THREE.PointLight(0xFFFFFF);

		self.light.position.x = 10;
		self.light.position.y = 250;
		self.light.position.z = 130;

		self.scene.add(self.light);

		// Init STL Loader
		self.loader = new THREE.STLLoader();

		self.loader.addEventListener('load', function (event)
		{
			var geometry = event.content;
			THREE.GeometryUtils.center(geometry);

			console.log(geometry.name);

			var mesh = new THREE.Mesh(geometry, self.material);

			self.meshes.push(mesh);
			self.scene.add(mesh);
		});

		// Load STL Files
		self.loader.load('./models/torso.stl');
		self.loader.load('./models/humerus.stl');
	}
	self.addWorld

	self.animate = function()
	{
		requestAnimationFrame(self.animate);

		for (var i = self.meshes.length - 1; i >= 0; i--)
		{
			//self.meshes[i].rotation.x += 0.01;
			self.meshes[i].rotation.y += 0.01;
		};

		self.renderer.render(self.scene, self.camera);
	}
}

var angle = 0;
var arm = new RobotArm();
arm.init();
arm.animate();