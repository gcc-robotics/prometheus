function RobotArm(viewModel)
{
	var self = this;
	
	self.viewModel = viewModel;

	self.container = $('#3d-prometheus-container');
	self.width = 970;
	self.height = 500;
	
	self.cameraIterator = 0;
	self.cameraDistance = 750;

	self.mesh = {};
	self.meshesToLoad = 0;
	self.meshesLoaded = 0;
	
	// Methods
	self.init = function()
	{
		// Enable auto-resizing of the renderer canvas
		self.startAutoResize();
		
		// Camera
		self.camera = new THREE.PerspectiveCamera(45, self.width / self.height, 0.1, 10000);
		self.camera.position.z = self.cameraDistance;
		self.camera.position.y = 500;

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

		// First Light
		var light = new THREE.PointLight(0xFFFFFF);

		light.position.x = 0;
		light.position.z = self.cameraDistance;
		light.position.y = self.cameraDistance * 1.5;
		self.scene.add(light);
		
		// Second Light
		light = light.clone();
		light.position.z = -self.cameraDistance;
		self.scene.add(light);
		
		// Ground Plane
		self.ground = new THREE.Mesh(new THREE.PlaneGeometry(5000, 5000, 40, 40), new THREE.MeshBasicMaterial({ wireframe: true, color: 0x999999 }));
		self.ground.rotation.x = Math.PI / 2;
		self.scene.add(self.ground);

		// Init STL Loader
		self.loader = new THREE.STLLoader();

		self.loader.addEventListener('load', function(event)
		{
			var geometry = event.content;
			var mesh = new THREE.Mesh(geometry, self.material);
			mesh.name = geometry.name;

			self.mesh[mesh.name] = mesh;
			
			self.meshesLoaded += 1;
			
			if(self.meshesLoaded == self.meshesToLoad)
			{
				self.loadingComplete();
			}
		});

		// Load STL Files
		self.meshesToLoad = 5;
		
		self.loader.load('./models/base.stl');
		self.loader.load('./models/torso.stl');
		self.loader.load('./models/humerus.stl');
		self.loader.load('./models/forearm-back.stl');
		self.loader.load('./models/forearm-front.stl');
	}
	
	self.startAutoResize = function()
	{
		window.addEventListener('resize', self.autoResizeCallback, false);
	}
	
	self.stopAutoResize = function()
	{
		window.removeEventListener('resize', self.autoResizeCallback);
	}
	
	self.autoResizeCallback = function()
	{
		self.renderer.setSize(self.container.width(), self.container.height());
		
		self.camera.aspect = self.container.width() / self.container.height();
		self.camera.updateProjectionMatrix();
	}
	
	self.addLine = function(parent, start, end)
	{
		// Line width doesn't work on windows
		var material = new THREE.LineBasicMaterial({linewidth: 3, color: 0x00FF00});
		
		var geometry = new THREE.Geometry;
		geometry.vertices.push(start);
		geometry.vertices.push(end);
		
		parent.add(new THREE.Line(geometry, material));
	}
	
	self.loadingComplete = function()
	{
		// Add base to scene
		self.scene.add(self.mesh['base']);
		
		// Move the axis of rotation for the base to it's center
		// Base Bounding box size: ~152.34, ~143.65, ~152.40
		self.mesh['base'].geometry.computeBoundingBox();
		var bb = self.mesh['base'].geometry.boundingBox;
		var offset = new THREE.Vector3();
		offset.addVectors(bb.min, bb.max);
		offset.multiplyScalar(-0.5);
		self.mesh['base'].geometry.applyMatrix(new THREE.Matrix4().makeTranslation(offset.x, 0, offset.z));
		self.mesh['base'].geometry.computeBoundingBox();
		
		// Add torso to base
		//self.scene.add(self.mesh['torso']);
		self.mesh['base'].add(self.mesh['torso']);
		
		// Move the axis of rotation for the torso to the center of it's base
		// Torso Bounding box size: ~152.36, ~325.48, ~152.40
		self.mesh['torso'].geometry.computeBoundingBox();
		var bb = self.mesh['torso'].geometry.boundingBox;
		var offset = new THREE.Vector3();
		offset.addVectors(bb.min, bb.max);
		offset.multiplyScalar(-0.5);
		self.mesh['torso'].geometry.applyMatrix(new THREE.Matrix4().makeTranslation(offset.x, 144, offset.z));
		self.mesh['torso'].geometry.computeBoundingBox();
		
		// Add humerus to torso
		self.mesh['torso'].add(self.mesh['humerus']);
		
		// Move the axis of rotation for the humerus
		// Humerus Bounding Box Size: ~114.30, ~352.95, ~217.95
		self.mesh['humerus'].geometry.applyMatrix(new THREE.Matrix4().makeTranslation(-57.15, -328, -193));
		self.mesh['humerus'].geometry.computeBoundingBox();
		
		// Move humerus to the correct position
		// The axis for jointing the torso to the humerus is 425 up from the center of base
		self.mesh['humerus'].position.y = 425;
		
		// Add the back of the forearm to the humerus
		self.mesh['humerus'].add(self.mesh['forearm-back']);
		
		// Move the axis of rotation for forearm-back
		// forearm-back Bounding Box Size: ~88.90, ~88.90, ~215.12
		self.mesh['forearm-back'].geometry.computeBoundingBox();
		var bb = self.mesh['forearm-back'].geometry.boundingBox;
		var offset = new THREE.Vector3();
		offset.addVectors(bb.min, bb.max);
		offset.multiplyScalar(-0.5);
		self.mesh['forearm-back'].geometry.applyMatrix(new THREE.Matrix4().makeTranslation(offset.x, offset.y, -195));
		self.mesh['forearm-back'].geometry.computeBoundingBox();
		
		// Move forearm-back to the correct position
		self.mesh['forearm-back'].position.y = -290;
		self.mesh['forearm-back'].position.z = -136;
		
		// Add the front of the forearm to the forearm-back
		self.mesh['forearm-back'].add(self.mesh['forearm-front']);
		
		// Move the axis of rotation for forearm-front
		THREE.GeometryUtils.center(self.mesh['forearm-front'].geometry);
		
		// Move forearm-front to the correct position
		self.mesh['forearm-front'].position.z = -266.5; // -265

		
		// Add lines for axes of rotation
		self.addLine(self.scene, new THREE.Vector3(0, 69, 0), new THREE.Vector3(0, 219, 0));
		self.addLine(self.mesh['torso'], new THREE.Vector3(100, 425, 0), new THREE.Vector3(-100, 425, 0));
		self.addLine(self.mesh['humerus'], new THREE.Vector3(100, -290, -135), new THREE.Vector3(-100, -290, -138));
		self.addLine(self.mesh['forearm-back'], new THREE.Vector3(0, 0, -238), new THREE.Vector3(0, 0, -138));

		console.log('3D: Mesh loading complete!');
		
		// Start the animation
		self.animate();	
	}
	
	self.animate = function()
	{
		requestAnimationFrame(self.animate);
		
		// Move meshes according to the current joint angles
		self.mesh['torso'].rotation.y = self.viewModel.joints()[0].currentAngle() / 180 * Math.PI;
		self.mesh['humerus'].rotation.x = self.viewModel.joints()[1].currentAngle() / 180 * Math.PI;
		self.mesh['forearm-back'].rotation.x = self.viewModel.joints()[2].currentAngle() / 180 * Math.PI;
		self.mesh['forearm-front'].rotation.z = self.viewModel.joints()[3].currentAngle() / 180 * Math.PI;
		
		// Rotate camera around 0,300,0
		// self.cameraIterator += 0.01;
		// self.camera.position.x = Math.sin(self.cameraIterator) * self.cameraDistance;
		// self.camera.position.z = Math.cos(self.cameraIterator) * self.cameraDistance;
		// self.camera.position.x = Math.sin(self.viewModel.joints()[4].setPoint() / 180 * Math.PI) * self.cameraDistance;
		// self.camera.position.z = Math.cos(self.viewModel.joints()[4].setPoint() / 180 * Math.PI) * self.cameraDistance;
		// self.camera.lookAt(new THREE.Vector3(0, 300, 0)); // 0, 300, 0
		
		self.renderer.render(self.scene, self.camera);
	}
}
